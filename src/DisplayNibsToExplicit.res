let f = (displayNibs: array<DisplayNib.t>, node: ConnectionNode.t, isSource: bool): array<
  ExplicitDisplayNib.t,
> =>
  Belt.Array.map(displayNibs, (displayNib: DisplayNib.t) => {
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
