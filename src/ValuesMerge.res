let f = (
  source: ConnectionSide.t,
  values: Belt.Map.String.t<Value.t>,
  sourceValues: SourceValues.t,
) => {
  Belt.Map.String.reduce(values, sourceValues, (sourceValues, nibID, value) =>
    Belt.Map.set(sourceValues, {ConnectionSide.node: source.node, nib: NibConnection(nibID)}, value)
  )
}
