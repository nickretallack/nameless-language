let f = (connectionSide: ConnectionSide.t): string =>
  ConnectionNodeToString.f(connectionSide.node) ++
  ("-" ++
  ConnectionNibToString.f(connectionSide.nib))
