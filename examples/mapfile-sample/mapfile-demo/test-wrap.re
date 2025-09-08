= Wrap/Foldオプションのテスト

== wrapオプションのテスト

//list[test1][wrap=60で折り返し][wrap=60,foldmark=↩]{
This is a very long line that should be wrapped at 60↩
  characters according to the wrap option setting in the↩
  list block parameters
//}

== 通常のリスト（オプションなし）

//list[test2][オプションなし]{
This is another very long line without any wrap options specified so it will not be wrapped automatically unless the LaTeX settings handle it
//}