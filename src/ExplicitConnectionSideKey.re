let f = (explicitConnectionSide: ExplicitConnectionSide.t): string =>
  Printf.sprintf(
    "nib-%s-%s-%s",
    ConnectionNodeKey.f(explicitConnectionSide.connectionSide.node),
    ConnectionNibKey.f(explicitConnectionSide.connectionSide.nib),
    explicitConnectionSide.isSource ? "source" : "sink",
  );
