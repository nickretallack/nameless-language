let f = (displayNibs: list<DisplayNib.t>, node: ConnectionNode.t, isSource: bool): list<
  ExplicitDisplayNib.t,
> =>
  Belt.List.map(displayNibs, (displayNib: DisplayNib.t) => {
    open ExplicitDisplayNib
    {
      name: displayNib.name,
      explicitConnectionSide: {
        connectionSide: {
          node: node,
          nib: displayNib.nib,
        },
        isSource: isSource,
      },
    }
  })
