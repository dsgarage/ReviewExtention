/**
 * textlint rule: no-deprecated-list
 * 非推奨のリスト記法（emlist, emlistnum, cmd, source）を検出し、
 * //list への移行を促す
 */

export default function noDeprecatedListRule(context) {
  const { Syntax, RuleError, report, fixer } = context;
  
  const deprecatedFormats = [
    {
      pattern: /^\/\/emlist(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?\{/gm,
      name: 'emlist',
      suggestion: '//list',
      fix: (match, label, caption) => {
        return `//list[${label || ''}][${caption || ''}]{`;
      }
    },
    {
      pattern: /^\/\/emlistnum(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?\{/gm,
      name: 'emlistnum',
      suggestion: '//list with [lineno=on]',
      fix: (match, label, caption) => {
        return `//list[${label || ''}][${caption || ''}][lineno=on]{`;
      }
    },
    {
      pattern: /^\/\/cmd(?:\[([^\]]*)\])?\{/gm,
      name: 'cmd',
      suggestion: '//list',
      fix: (match, caption) => {
        return `//list[][${caption || 'コマンド'}][fontsize=small]{`;
      }
    },
    {
      pattern: /^\/\/source(?:\[([^\]]*)\])?(?:\[([^\]]*)\])?\{/gm,
      name: 'source',
      suggestion: '//list with [filename=...]',
      fix: (match, file, caption) => {
        const options = file ? `filename=${file}` : '';
        return `//list[][${caption || ''}]${options ? `[${options}]` : ''}{`;
      }
    }
  ];
  
  return {
    [Syntax.Document](node) {
      const text = context.getSource(node);
      
      deprecatedFormats.forEach(format => {
        let match;
        format.pattern.lastIndex = 0; // Reset regex state
        
        while ((match = format.pattern.exec(text)) !== null) {
          const index = match.index;
          const matchedText = match[0];
          const line = text.substring(0, index).split('\n').length;
          const column = index - text.lastIndexOf('\n', index - 1);
          
          const message = new RuleError(
            `非推奨のリスト記法 '${format.name}' が検出されました。${format.suggestion} を使用してください。`,
            {
              line: line,
              column: column,
              index: index,
              fix: fixer.replaceTextRange(
                [index, index + matchedText.length],
                format.fix(...match.slice(1))
              )
            }
          );
          
          report(node, message);
        }
      });
    }
  };
}