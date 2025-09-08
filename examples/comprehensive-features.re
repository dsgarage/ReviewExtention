= 統合機能デモ：Re:VIEW拡張機能の実践例

== はじめに

このドキュメントでは、Re:VIEW 5.8対応の拡張機能を組み合わせた実践的な使用例を紹介します。

//info[対象読者]{
 * Re:VIEWで技術文書を作成している方
 * より表現力豊かなドキュメントを作成したい方
 * 既存のRe:VIEW文書を5.8対応に移行したい方
//}

== Webアプリケーション開発ガイド

=== プロジェクトのセットアップ

//important[前提条件]{
開発を始める前に、以下の環境が整っていることを確認してください：
//}

//list[requirements][必要な環境][lang=yaml,filename=requirements.txt]{
# 開発環境要件
node: ">=18.0.0"
npm: ">=9.0.0"
git: ">=2.40.0"

# データベース
postgresql: "15.x"
redis: "7.x"

# オプション
docker: "24.x"
docker-compose: "2.x"
//}

//tip[Dockerを使用する場合]{
Dockerを使用すると、環境構築が簡単になります：

@<code>{docker-compose up -d}

これで必要なサービスがすべて起動します。
//}

=== 初期設定

//list[setup][セットアップスクリプト][lang=bash,lineno=on,highlight=5-7]{
#!/bin/bash
# プロジェクトセットアップスクリプト

# 1. 依存関係のインストール
echo "Installing dependencies..."
npm install
npm run prepare

# 2. 環境変数の設定
cp .env.example .env
echo "Please edit .env file with your configuration"

# 3. データベースの初期化
npm run db:migrate
npm run db:seed
//}

//warning[環境変数の設定]{
@<code>{.env}ファイルには機密情報が含まれます。
絶対にGitにコミットしないでください。
//}

=== アーキテクチャ概要

//note[architecture_overview][システムアーキテクチャ]{
本アプリケーションは、以下の3層アーキテクチャを採用しています：

 1. プレゼンテーション層（React + TypeScript）
 2. ビジネスロジック層（Node.js + Express）
 3. データアクセス層（PostgreSQL + Redis）

詳細は@<chap>{architecture}を参照してください。
//}

//list[server_code][サーバー実装例][lang=typescript,filename=src/server.ts,wrap=80,foldmark=↩]{
import express, { Application, Request, Response, NextFunction } from 'express';
import cors from 'cors';
import helmet from 'helmet';
import morgan from 'morgan';
import { errorHandler } from './middleware/errorHandler';
import { authRouter } from './routes/auth';
import { apiRouter } from './routes/api';

const app: Application = express();

// ミドルウェアの設定
app.use(helmet()); // セキュリティヘッダーの設定
app.use(cors()); // CORS対応
app.use(morgan('combined')); // ログ出力
app.use(express.json()); // JSONパース
app.use(express.urlencoded({ extended: true })); // URLエンコードパース

// ルーティング
app.use('/auth', authRouter);
app.use('/api', apiRouter);

// エラーハンドリング
app.use(errorHandler);

// サーバー起動
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
//}

== API実装ガイド

=== RESTful API設計

//info[REST原則]{
RESTful APIは以下の原則に従って設計します：

 * リソース指向のURL設計
 * HTTPメソッドの適切な使用
 * ステートレスな通信
 * 統一されたインターフェース
//}

//list[api_routes][APIエンドポイント定義][lang=javascript,lineno=on,filename=routes/users.js]{
const router = express.Router();

// ユーザー一覧取得
router.get('/users', async (req, res) => {
    const users = await User.findAll();
    res.json(users);
});

// ユーザー詳細取得
router.get('/users/:id', async (req, res) => {
    const user = await User.findById(req.params.id);
    if (!user) {
        return res.status(404).json({ error: 'User not found' });
    }
    res.json(user);
});

// ユーザー作成
router.post('/users', validateUser, async (req, res) => {
    const user = await User.create(req.body);
    res.status(201).json(user);
});

// ユーザー更新
router.put('/users/:id', validateUser, async (req, res) => {
    const user = await User.update(req.params.id, req.body);
    res.json(user);
});

// ユーザー削除
router.delete('/users/:id', async (req, res) => {
    await User.delete(req.params.id);
    res.status(204).send();
});
//}

//caution[削除操作の注意]{
削除操作は取り消せません。実装時は以下を検討してください：

 * 論理削除（soft delete）の採用
 * 削除前の確認ダイアログ
 * 削除権限の厳格な管理
//}

=== エラーハンドリング

//important[統一されたエラーレスポンス]{
すべてのAPIエラーは統一されたフォーマットで返却してください。
//}

//list[error_format][エラーレスポンス形式][lang=typescript,filename=types/error.ts]{
interface ApiError {
    error: {
        code: string;        // エラーコード（例: "USER_NOT_FOUND"）
        message: string;     // 人間が読めるメッセージ
        details?: any;       // 追加の詳細情報（オプション）
        timestamp: string;   // エラー発生時刻
        path: string;        // リクエストパス
    };
}

// エラーレスポンスの例
{
    "error": {
        "code": "VALIDATION_ERROR",
        "message": "入力データが不正です",
        "details": {
            "email": "有効なメールアドレスを入力してください",
            "password": "パスワードは8文字以上必要です"
        },
        "timestamp": "2024-01-01T00:00:00.000Z",
        "path": "/api/users"
    }
}
//}

//memo[エラーコード一覧]{
エラーコードの一覧は別途ドキュメント化する予定。
統一されたコード体系により、クライアント側での
エラーハンドリングが容易になる。
//}

== データベース設計

=== スキーマ定義

//list[schema][データベーススキーマ][lang=sql,filename=schema.sql,lineno=on,highlight=1-7]{
-- ユーザーテーブル
CREATE TABLE users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    email VARCHAR(255) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- インデックス
CREATE INDEX idx_users_email ON users(email);

-- 投稿テーブル
CREATE TABLE posts (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID REFERENCES users(id) ON DELETE CASCADE,
    title VARCHAR(255) NOT NULL,
    content TEXT,
    published_at TIMESTAMP,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- インデックス
CREATE INDEX idx_posts_user_id ON posts(user_id);
CREATE INDEX idx_posts_published_at ON posts(published_at);
//}

//warning[インデックスの使用]{
インデックスは検索性能を向上させますが、
書き込み性能に影響を与える可能性があります。
必要最小限のインデックスのみ作成してください。
//}

=== マイグレーション

//tip[マイグレーションのベストプラクティス]{
 * 必ず開発環境でテストしてから本番適用
 * ロールバック可能な設計にする
 * 大規模な変更は段階的に実施
//}

//list[migration][マイグレーションファイル][lang=javascript,filename=migrations/001_add_user_profile.js]{
exports.up = async (knex) => {
    // プロフィールテーブルの追加
    await knex.schema.createTable('user_profiles', (table) => {
        table.uuid('user_id').primary();
        table.foreign('user_id').references('users.id').onDelete('CASCADE');
        table.string('display_name', 100);
        table.text('bio');
        table.string('avatar_url');
        table.timestamps(true, true);
    });
    
    // 既存ユーザーのプロフィール作成
    const users = await knex('users').select('id');
    const profiles = users.map(user => ({
        user_id: user.id,
        display_name: 'User',
        created_at: knex.fn.now(),
        updated_at: knex.fn.now()
    }));
    
    if (profiles.length > 0) {
        await knex('user_profiles').insert(profiles);
    }
};

exports.down = async (knex) => {
    await knex.schema.dropTableIfExists('user_profiles');
};
//}

== セキュリティ実装

=== 認証と認可

//important[セキュリティの重要性]{
適切な認証・認可の実装は、アプリケーションの
セキュリティの基盤となります。
//}

//list[auth_middleware][認証ミドルウェア][lang=typescript,filename=middleware/auth.ts,wrap=70]{
import jwt from 'jsonwebtoken';
import { Request, Response, NextFunction } from 'express';

interface AuthRequest extends Request {
    user?: {
        id: string;
        email: string;
        role: string;
    };
}

export const authenticate = async (
    req: AuthRequest,
    res: Response,
    next: NextFunction
) => {
    try {
        const token = req.headers.authorization?.replace('Bearer ', '');
        
        if (!token) {
            throw new Error('認証トークンが見つかりません');
        }
        
        const decoded = jwt.verify(token, process.env.JWT_SECRET!) as any;
        
        // ユーザー情報をリクエストに追加
        req.user = {
            id: decoded.id,
            email: decoded.email,
            role: decoded.role
        };
        
        next();
    } catch (error) {
        res.status(401).json({
            error: {
                code: 'UNAUTHORIZED',
                message: '認証が必要です'
            }
        });
    }
};

export const authorize = (...roles: string[]) => {
    return (req: AuthRequest, res: Response, next: NextFunction) => {
        if (!req.user) {
            return res.status(401).json({
                error: {
                    code: 'UNAUTHORIZED',
                    message: '認証が必要です'
                }
            });
        }
        
        if (!roles.includes(req.user.role)) {
            return res.status(403).json({
                error: {
                    code: 'FORBIDDEN',
                    message: '権限がありません'
                }
            });
        }
        
        next();
    };
};
//}

//caution[JWTシークレットの管理]{
JWT_SECRETは強力なランダム文字列を使用し、
環境変数で管理してください。
本番環境では定期的に更新することを推奨します。
//}

=== 入力検証

//warning[SQLインジェクション対策]{
ユーザー入力は必ず検証・サニタイズしてください。
//}

//list[validation][入力検証の実装][lang=javascript,filename=validators/user.js,lineno=on]{
const { body, validationResult } = require('express-validator');

const userValidationRules = () => {
    return [
        body('email')
            .isEmail()
            .normalizeEmail()
            .withMessage('有効なメールアドレスを入力してください'),
        
        body('password')
            .isLength({ min: 8 })
            .withMessage('パスワードは8文字以上必要です')
            .matches(/^(?=.*[A-Za-z])(?=.*\d)/)
            .withMessage('パスワードは英数字を含む必要があります'),
        
        body('age')
            .optional()
            .isInt({ min: 0, max: 150 })
            .withMessage('年齢は0〜150の数値で入力してください')
    ];
};

const validate = (req, res, next) => {
    const errors = validationResult(req);
    
    if (!errors.isEmpty()) {
        return res.status(400).json({
            error: {
                code: 'VALIDATION_ERROR',
                message: '入力データが不正です',
                details: errors.array()
            }
        });
    }
    
    next();
};

module.exports = {
    userValidationRules,
    validate
};
//}

== テスト戦略

=== ユニットテスト

//info[テストの重要性]{
適切なテストカバレッジは、コードの品質と
信頼性を保証する重要な要素です。
//}

//list[unit_test][ユニットテストの例][lang=javascript,filename=tests/user.test.js,lineno=on,foldable=on]{
const { expect } = require('chai');
const sinon = require('sinon');
const UserService = require('../services/UserService');
const UserRepository = require('../repositories/UserRepository');

describe('UserService', () => {
    let userService;
    let userRepositoryStub;
    
    beforeEach(() => {
        userRepositoryStub = sinon.stub(UserRepository);
        userService = new UserService(userRepositoryStub);
    });
    
    afterEach(() => {
        sinon.restore();
    });
    
    describe('createUser', () => {
        it('should create a new user successfully', async () => {
            // Arrange
            const userData = {
                email: 'test@example.com',
                password: 'Password123'
            };
            
            const expectedUser = {
                id: 'user_123',
                email: userData.email,
                createdAt: new Date()
            };
            
            userRepositoryStub.create.resolves(expectedUser);
            
            // Act
            const result = await userService.createUser(userData);
            
            // Assert
            expect(result).to.deep.equal(expectedUser);
            expect(userRepositoryStub.create.calledOnce).to.be.true;
            expect(userRepositoryStub.create.calledWith(userData)).to.be.true;
        });
        
        it('should throw error for duplicate email', async () => {
            // Arrange
            const userData = {
                email: 'existing@example.com',
                password: 'Password123'
            };
            
            userRepositoryStub.create.rejects(new Error('Duplicate email'));
            
            // Act & Assert
            await expect(userService.createUser(userData))
                .to.be.rejectedWith('Duplicate email');
        });
    });
    
    describe('getUserById', () => {
        it('should return user when found', async () => {
            // Arrange
            const userId = 'user_123';
            const expectedUser = {
                id: userId,
                email: 'test@example.com'
            };
            
            userRepositoryStub.findById.resolves(expectedUser);
            
            // Act
            const result = await userService.getUserById(userId);
            
            // Assert
            expect(result).to.deep.equal(expectedUser);
        });
        
        it('should return null when user not found', async () => {
            // Arrange
            userRepositoryStub.findById.resolves(null);
            
            // Act
            const result = await userService.getUserById('nonexistent');
            
            // Assert
            expect(result).to.be.null;
        });
    });
});
//}

//tip[テストのコツ]{
 * AAAパターン（Arrange-Act-Assert）を使用
 * 1つのテストで1つの振る舞いをテスト
 * テストは独立して実行可能にする
//}

== デプロイとモニタリング

=== CI/CDパイプライン

//list[ci_config][GitHub Actions設定][lang=yaml,filename=.github/workflows/ci.yml,wrap=80]{
name: CI/CD Pipeline

on:
  push:
    branches: [main, develop]
  pull_request:
    branches: [main]

jobs:
  test:
    runs-on: ubuntu-latest
    
    services:
      postgres:
        image: postgres:15
        env:
          POSTGRES_PASSWORD: postgres
        options: >-
          --health-cmd pg_isready
          --health-interval 10s
          --health-timeout 5s
          --health-retries 5
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Setup Node.js
        uses: actions/setup-node@v3
        with:
          node-version: '18'
          cache: 'npm'
      
      - name: Install dependencies
        run: npm ci
      
      - name: Run linter
        run: npm run lint
      
      - name: Run tests
        run: npm test
        env:
          DATABASE_URL: postgres://postgres:postgres@localhost:5432/test_db
      
      - name: Build application
        run: npm run build
      
      - name: Deploy to staging
        if: github.ref == 'refs/heads/develop'
        run: npm run deploy:staging
        env:
          DEPLOY_TOKEN: ${{ secrets.STAGING_DEPLOY_TOKEN }}
      
      - name: Deploy to production
        if: github.ref == 'refs/heads/main'
        run: npm run deploy:production
        env:
          DEPLOY_TOKEN: ${{ secrets.PRODUCTION_DEPLOY_TOKEN }}
//}

//notice[デプロイ通知]{
デプロイが完了すると、Slackに通知が送信されます。
エラーが発生した場合は、担当者にメンションが飛びます。
//}

=== モニタリング

//important[監視項目]{
以下の項目を継続的に監視してください：

 * アプリケーションのレスポンスタイム
 * エラーレート
 * CPU/メモリ使用率
 * データベースのクエリパフォーマンス
//}

//list[monitoring][モニタリング設定][lang=javascript,filename=monitoring/metrics.js]{
const prometheus = require('prom-client');
const collectDefaultMetrics = prometheus.collectDefaultMetrics;

// デフォルトメトリクスの収集
collectDefaultMetrics({ timeout: 5000 });

// カスタムメトリクス
const httpRequestDuration = new prometheus.Histogram({
    name: 'http_request_duration_seconds',
    help: 'Duration of HTTP requests in seconds',
    labelNames: ['method', 'route', 'status_code']
});

const httpRequestTotal = new prometheus.Counter({
    name: 'http_requests_total',
    help: 'Total number of HTTP requests',
    labelNames: ['method', 'route', 'status_code']
});

// ミドルウェア
const metricsMiddleware = (req, res, next) => {
    const start = Date.now();
    
    res.on('finish', () => {
        const duration = (Date.now() - start) / 1000;
        const labels = {
            method: req.method,
            route: req.route?.path || 'unknown',
            status_code: res.statusCode
        };
        
        httpRequestDuration.observe(labels, duration);
        httpRequestTotal.inc(labels);
    });
    
    next();
};

module.exports = {
    register: prometheus.register,
    metricsMiddleware
};
//}

//memo[アラート設定]{
重要なメトリクスにはアラートを設定し、
異常を早期に検知できるようにする。
//}

== まとめ

//note[summary][本ドキュメントのまとめ]{
本ドキュメントでは、Re:VIEW拡張機能を活用した
実践的なドキュメント作成例を紹介しました。

主な機能：
 * リスト機能の拡張（行番号、ハイライト、折り返し）
 * Note/Miniblockによる情報の構造化
 * 実践的なコード例とベストプラクティス

これらの機能を組み合わせることで、
より読みやすく、保守しやすいドキュメントを
作成することができます。
//}

//tip[さらなる学習]{
Re:VIEWの詳細な仕様については、
公式ドキュメントを参照してください：

@<href>{https://reviewml.org/}
//}

//important[フィードバック歓迎]{
本拡張機能へのフィードバックや要望は、
GitHubリポジトリのIssueでお待ちしています。
//}