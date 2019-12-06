let f =
    (displayNibs: list(DisplayNib.t), node: ConnectionNode.t, isSource: bool)
    : list(DisplayNibExplicit.t) =>
  Belt.List.map(displayNibs, (displayNib: DisplayNib.t) =>
    DisplayNibExplicit.{
      name: displayNib.name,
      explicitConnectionSide: {
        connectionSide: {
          node,
          nib: displayNib.nib,
        },
        isSource,
      },
    }
  );
