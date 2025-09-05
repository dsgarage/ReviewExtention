#!/usr/bin/env node

/**
 * Re:VIEW Preprocessor - Convert differential tags to Re:VIEW 5.8 compatible format
 * Version 0.2.0 - Added list unification and extended options
 */

import { program } from 'commander';
import { globby } from 'globby';
import fs from 'fs/promises';
import path from 'path';
import chalk from 'chalk';

// リストオプション解析
function parseListOptions(optionString) {
  if (!optionString) return {};
  
  const options = {};
  const parts = optionString.split(',');
  
  for (const part of parts) {
    const [key, value] = part.split('=');
    if (key) {
      options[key.trim()] = value ? value.trim() : 'on';
    }
  }
  
  return options;
}

// リストオプションを文字列に戻す
function stringifyListOptions(options) {
  if (!options || Object.keys(options).length === 0) return '';
  
  return Object.entries(options)
    .map(([key, value]) => value === 'on' ? key : `${key}=${value}`)
    .join(',');
}

// リスト変換ルール（拡張版）
function convertListWithOptions(match, type, label, caption, options, content) {
  // オプション解析
  const opts = parseListOptions(options);
  
  // 拡張オプションの処理
  if (opts.captionpos === 'none') {
    caption = ''; // キャプションを非表示
  }
  
  if (opts.filename) {
    // ファイル名バッジを追加（コメントとして記録）
    content = `#@# filename: ${opts.filename}\n${content}`;
  }
  
  if (opts.highlight) {
    // 強調行の記録
    content = `#@# highlight: ${opts.highlight}\n${content}`;
  }
  
  if (opts.diffformat && opts.diffformat !== 'none') {
    // 差分フォーマットの記録
    content = `#@# diffformat: ${opts.diffformat}\n${content}`;
  }
  
  // 折り返し関連のオプション処理
  if (opts.fold === 'off') {
    // 折り返し無効（Re:VIEWのデフォルト動作）
    // そのまま維持
  } else if (opts.fold === 'on' || opts.wrap === 'on') {
    // 折り返し有効（デフォルト）
    if (!opts.foldmark) {
      opts.foldmark = '↩'; // デフォルトの折り返し記号
    }
  }
  
  // 長い行の処理（wrap=<number> で指定された場合）
  if (opts.wrap && opts.wrap !== 'on' && opts.wrap !== 'off') {
    const wrapLength = parseInt(opts.wrap);
    if (!isNaN(wrapLength) && wrapLength > 0) {
      // 指定文字数で折り返し
      content = wrapLongLines(content, wrapLength, opts.foldmark || '↩');
    }
  }
  
  // オプション文字列の再構築
  const optStr = stringifyListOptions(opts);
  const optPart = optStr ? `[${optStr}]` : '';
  
  return `//list[${label || ''}][${caption || ''}]${optPart}{${content}//}`;
}

// 長い行を折り返す関数
function wrapLongLines(content, maxLength, foldMark) {
  const lines = content.split('\n');
  const wrappedLines = [];
  
  for (const line of lines) {
    if (line.length <= maxLength) {
      wrappedLines.push(line);
    } else {
      // 長い行を折り返し
      let remaining = line;
      while (remaining.length > 0) {
        if (remaining.length <= maxLength) {
          wrappedLines.push(remaining);
          break;
        }
        
        // 単語境界で分割を試みる
        let breakPoint = maxLength;
        const lastSpace = remaining.lastIndexOf(' ', maxLength);
        if (lastSpace > maxLength * 0.7) { // 70%以降にスペースがあれば使用
          breakPoint = lastSpace;
        }
        
        wrappedLines.push(remaining.substring(0, breakPoint) + foldMark);
        remaining = '  ' + remaining.substring(breakPoint).trim(); // インデント追加
      }
    }
  }
  
  return wrappedLines.join('\n');
}

// 変換ルール定義
const rules = [
  // ===== インラインタグ変換 =====
  {
    name: 'B to strong',
    type: 'regex',
    from: /@@<B>\{([^}]*)\}/g,
    to: '@@<strong>{$1}',
    description: 'Convert @@<B> to @@<strong>'
  },
  {
    name: 'nop literal',
    type: 'regex',
    from: /@@<nop>\{([^}]*)\}/g,
    to: '{{{$1}}}',
    description: 'Convert @@<nop> to literal brackets'
  },
  {
    name: 'term comma to pipe',
    type: 'regex',
    from: /@@<term>\{([^,{}]+),([^{}]+)\}/g,
    to: '@@<term>{$1|$2}',
    description: 'Unify term tag delimiter from comma to pipe'
  },
  
  // ===== ブロックタグ変換 =====
  {
    name: 'abstract to lead',
    type: 'block',
    from: /^\/\/abstract\{([\s\S]*?)^\/\/\}/gm,
    to: '//lead{$1//}',
    description: 'Convert //abstract to //lead'
  },
  {
    name: 'vspace to note',
    type: 'regex',
    from: /^\/\/vspace\[([+-]?\d+)\]\s*$/gm,
    to: '//note[版面調整:vspace=$1]',
    description: 'Convert //vspace to note directive'
  },
  {
    name: 'needvspace to note',
    type: 'regex',
    from: /^\/\/needvspace\[([+-]?\d+)\]\s*$/gm,
    to: '//note[版面調整:needvspace=$1]',
    description: 'Convert //needvspace to note directive'
  },
  {
    name: 'sideimage to image+note',
    type: 'regex',
    from: /^\/\/sideimage\[([^\]]*)\]\[([^\]]*)\]\s*$/gm,
    to: '//image[$1][$2]\n//note[サイド注記:横組はスタイルで調整]',
    description: 'Convert //sideimage to //image with note'
  },
  
  // ===== リストタグの統一（新規追加） =====
  {
    name: 'emlist to list',
    type: 'block',
    from: /^\/\/emlist(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?\{([\s\S]*?)^\/\/\}/gm,
    to: (match, label, caption, options, content) => {
      return `//list[${label || ''}][${caption || ''}]${options ? `[${options}]` : ''}{${content}//}`;
    },
    description: 'Convert //emlist to //list'
  },
  {
    name: 'emlistnum to listnum',
    type: 'block',
    from: /^\/\/emlistnum(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?\{([\s\S]*?)^\/\/\}/gm,
    to: (match, label, caption, options, content) => {
      // listnum は lineno=on がデフォルト
      const opts = parseListOptions(options);
      if (!opts.lineno) opts.lineno = 'on';
      const optStr = stringifyListOptions(opts);
      return `//list[${label || ''}][${caption || ''}][${optStr}]{${content}//}`;
    },
    description: 'Convert //emlistnum to //list with lineno'
  },
  {
    name: 'cmd to list',
    type: 'block',
    from: /^\/\/cmd(?:\[([^\]]*)\])?\{([\s\S]*?)^\/\/\}/gm,
    to: (match, caption, content) => {
      return `//list[][${caption || 'コマンド'}][fontsize=small]{${content}//}`;
    },
    description: 'Convert //cmd to //list'
  },
  {
    name: 'source to list',
    type: 'block',
    from: /^\/\/source(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?\{([\s\S]*?)^\/\/\}/gm,
    to: (match, file, caption, content) => {
      const opts = {};
      if (file) opts.filename = file;
      const optStr = stringifyListOptions(opts);
      return `//list[][${caption || ''}]${optStr ? `[${optStr}]` : ''}{${content}//}`;
    },
    description: 'Convert //source to //list'
  },
  
  // ===== 拡張リストオプションの処理 =====
  {
    name: 'list with extended options',
    type: 'custom',
    process: (content) => {
      // //list[label][caption][options]{...} のオプションを拡張処理
      const listPattern = /^\/\/list\[([^\]]*)\]\[([^\]]*)\]\[([^\]]*)\]\{([\s\S]*?)^\/\/\}/gm;
      
      return content.replace(listPattern, (match, label, caption, options, body) => {
        // オプションを正規化（lineno → lineno=on）
        if (options) {
          const normalizedOptions = options.split(',').map(opt => {
            opt = opt.trim();
            if (opt === 'lineno') return 'lineno=on';
            if (opt === 'fold') return 'fold=on';
            if (opt === 'widecharfit') return 'widecharfit=on';
            if (opt === 'copybutton') return 'copybutton=on';
            if (opt === 'foldable') return 'foldable=on';
            if (opt === 'anchorlinks') return 'anchorlinks=on';
            return opt;
          }).join(',');
          return convertListWithOptions(match, 'list', label, caption, normalizedOptions, body);
        }
        return convertListWithOptions(match, 'list', label, caption, options, body);
      });
    },
    description: 'Process extended list options'
  }
];

// 統計情報
let stats = {
  filesProcessed: 0,
  totalReplacements: 0,
  ruleStats: {}
};

// ファイル処理関数
async function processFile(filePath, outputDir, verbose) {
  const content = await fs.readFile(filePath, 'utf8');
  let processed = content;
  let fileReplacements = 0;
  
  for (const rule of rules) {
    if (!stats.ruleStats[rule.name]) {
      stats.ruleStats[rule.name] = 0;
    }
    
    const before = processed;
    
    if (rule.type === 'regex' || rule.type === 'block') {
      if (typeof rule.to === 'function') {
        processed = processed.replace(rule.from, rule.to);
      } else {
        processed = processed.replace(rule.from, rule.to);
      }
    } else if (rule.type === 'custom' && rule.process) {
      processed = rule.process(processed);
    }
    
    if (before !== processed) {
      const replacements = 1; // 簡易カウント
      stats.ruleStats[rule.name] += replacements;
      fileReplacements += replacements;
      
      if (verbose) {
        console.log(chalk.gray(`  Applied: ${rule.name}`));
      }
    }
  }
  
  // 出力ファイルパスの生成
  const relativePath = path.relative(process.cwd(), filePath);
  const outputPath = path.join(outputDir, relativePath);
  
  // ディレクトリ作成と書き込み
  await fs.mkdir(path.dirname(outputPath), { recursive: true });
  await fs.writeFile(outputPath, processed, 'utf8');
  
  stats.filesProcessed++;
  stats.totalReplacements += fileReplacements;
  
  return fileReplacements;
}

// メインプログラム
program
  .version('0.2.0')
  .description('Preprocess Re:VIEW files to convert differential tags with list extensions')
  .argument('<patterns...>', 'Glob patterns for Re:VIEW files (e.g., "articles/**/*.re")')
  .option('-o, --out-dir <dir>', 'Output directory', '.out')
  .option('-v, --verbose', 'Verbose output', false)
  .option('--dry-run', 'Perform a dry run without writing files', false)
  .option('--stats', 'Show conversion statistics', false)
  .option('--list-options', 'Show available list options', false)
  .action(async (patterns, options) => {
    try {
      // リストオプション一覧表示
      if (options.listOptions) {
        console.log(chalk.cyan('Available List Options:'));
        console.log(chalk.white(`
  captionpos=top|bottom|none  - Caption position
  filename=<name>              - Show filename badge
  highlight=1,3-5              - Highlight lines
  lineno=on|off|<number>       - Line numbers
  fontsize=<size>              - Font size
  diffformat=inline|side       - Diff display format
  copybutton=on|off            - Copy button (HTML)
  foldable=on|off              - Foldable code (HTML)
  indent=<width>               - Indent guides
  widecharfit=on|off           - Wide character fitting
  lang=<language>              - Syntax highlighting language
        `));
        process.exit(0);
      }
      
      console.log(chalk.cyan('Re:VIEW Preprocessor v0.2.0 - Starting conversion...'));
      
      // ファイルの検索
      const files = await globby(patterns, { dot: false });
      
      if (files.length === 0) {
        console.log(chalk.yellow('No files found matching the patterns.'));
        process.exit(0);
      }
      
      console.log(chalk.green(`Found ${files.length} file(s) to process`));
      
      // 各ファイルの処理
      for (const file of files) {
        if (options.verbose) {
          console.log(chalk.blue(`Processing: ${file}`));
        }
        
        if (!options.dryRun) {
          const replacements = await processFile(file, options.outDir, options.verbose);
          if (replacements > 0) {
            console.log(chalk.green(`✓ ${file} (${replacements} replacements)`));
          } else if (options.verbose) {
            console.log(chalk.gray(`✓ ${file} (no changes)`));
          }
        } else {
          console.log(chalk.gray(`[DRY RUN] Would process: ${file}`));
        }
      }
      
      // 統計情報の表示
      if (options.stats || options.verbose) {
        console.log(chalk.cyan('\n=== Conversion Statistics ==='));
        console.log(chalk.white(`Files processed: ${stats.filesProcessed}`));
        console.log(chalk.white(`Total replacements: ${stats.totalReplacements}`));
        
        if (Object.keys(stats.ruleStats).length > 0) {
          console.log(chalk.cyan('\nRule applications:'));
          for (const [rule, count] of Object.entries(stats.ruleStats)) {
            if (count > 0) {
              console.log(chalk.gray(`  ${rule}: ${count}`));
            }
          }
        }
      }
      
      if (!options.dryRun) {
        console.log(chalk.green(`\n✓ Preprocessing complete! Output written to: ${options.outDir}`));
      } else {
        console.log(chalk.yellow('\n[DRY RUN] No files were modified.'));
      }
      
    } catch (error) {
      console.error(chalk.red('Error:'), error.message);
      if (options.verbose) {
        console.error(error.stack);
      }
      process.exit(1);
    }
  });

// ヘルプ表示のカスタマイズ
program.on('--help', () => {
  console.log('');
  console.log('Examples:');
  console.log('  $ review-preprocess "articles/**/*.re"');
  console.log('  $ review-preprocess "chapters/*.re" -o build/preprocessed');
  console.log('  $ review-preprocess "**/*.re" --dry-run --stats');
  console.log('  $ review-preprocess --list-options  # Show available list options');
  console.log('');
  console.log('Conversion Rules:');
  rules.forEach(rule => {
    if (rule.description) {
      console.log(`  - ${rule.description}`);
    }
  });
});

program.parse();