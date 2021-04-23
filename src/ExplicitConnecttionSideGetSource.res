let f = (
  explicitConnectionSide: ExplicitConnectionSide.t,
  connections: ConnectionMap.t,
): ConnectionSide.t =>
  explicitConnectionSide.isSource
    ? explicitConnectionSide.connectionSide
    : Belt.Map.getExn(connections, explicitConnectionSide.connectionSide)
