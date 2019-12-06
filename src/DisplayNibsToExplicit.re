let f =
    (displayNibs: list(DisplayNib.t), node: ConnectionNode.t, isSource: bool)
    : list(ExplicitDisplayNib.t) =>
  Belt.List.map(displayNibs, (displayNib: DisplayNib.t) =>
    ExplicitDisplayNib.{
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
