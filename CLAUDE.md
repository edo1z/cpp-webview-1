# C++ Webview プロジェクト

このプロジェクトはC++とWebviewを使用したGUIアプリケーション開発プロジェクトです。

## 開発環境
- OS: Windows 11 + WSL2 (Ubuntu)
- 言語: C++
- GUI: Webview (Qt6は使用しません)

## ビルド設定
- ビルドシステム: CMakeを使用
- C++標準: C++17以上を推奨
- コンパイラ: GCC/Clang

## プロジェクト構造
```
.
├── src/           # ソースコード
├── include/       # ヘッダーファイル
├── web/           # HTMLやJavaScript等のWebリソース
├── build/         # ビルド出力ディレクトリ
└── tests/         # テストコード
```

## コーディング規約
- インデント: スペース4つ
- 命名規則:
  - クラス名: PascalCase
  - 関数名: camelCase
  - 変数名: snake_case
  - 定数: UPPER_SNAKE_CASE

## Webview実装時の注意事項
- webview2ライブラリを使用
- JavaScriptとC++間の通信は適切にバインディングを設定
- セキュリティ: 外部URLへのアクセスは必要最小限に制限

## ビルド手順
```bash
mkdir build
cd build
cmake ..
make
```

## テスト
- Google Testを使用してユニットテストを実装
- テスト実行: `./build/tests/test_runner`

## 開発プラクティス

### TDD（テスト駆動開発）
**t-wada流のTDDで開発を進める**
1. **Red**: 失敗するテストを書く
2. **Green**: テストを通す最小限の実装
3. **Refactor**: コードを改善

### コミット粒度
- 機能単位で小まめにコミット
- 各ステップ（Red→Green→Refactor）でコミットすることを推奨

### 品質保証
- **コミット前の必須確認事項**:
  - 全てのテストがパスすることを確認
  - テストをパスさせるための無益な修正は行わない
  - ビルドエラー・警告を修正してからコミット
  - 修正したファイルに対するLSPツールのエラー・警告を全てチェックして、なくなるように修正する
- メモリリークチェックを定期的に実施（Valgrind等を使用）