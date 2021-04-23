let f = (
  sink: ConnectionSide.t,
  columns: list<list<NodeWithID.t>>,
  nodeToColumnIndex: Belt.Map.String.t<int>,
  definitions: DefinitionMap.t,
  display: DefinitionDisplay.t,
): (int, int, int) =>
  switch sink.node {
  | GraphConnection =>
    let sinkColumnIndex = -1
    let sinkIndexInColumn = 0
    let sinkIndex = switch sink.nib {
    | NibConnection(nibID) => ListFindIndexExn.f(display.outputOrdering, nibID)
    | _ => raise(Exception.TODO("RankConnection non-nib connection"))
    }

    (-sinkColumnIndex, sinkIndexInColumn, sinkIndex)
  | NodeConnection(sinkNodeID) =>
    let sinkColumnIndex = Belt.Map.String.getExn(nodeToColumnIndex, sinkNodeID) // 1st metric
    let column = Belt.List.getExn(columns, sinkColumnIndex)
    let sinkIndexInColumn = ListFindByIndexExn.f(column, node => node.id == sinkNodeID) // 2nd metric

    let node = Belt.List.getExn(column, sinkIndexInColumn)
    let nodeDisplay = NodeGetDisplayNibs.f(node.node, definitions, "en")

    let nibCollection = NodeIsFunctionDefinition.f(node.node)
      ? nodeDisplay.internalOutputs
      : nodeDisplay.inputs
    let sinkIndex = ListFindByIndexExn.f(nibCollection, ({nib}) => nib == sink.nib) // 3rd metric
    (-sinkColumnIndex, sinkIndexInColumn, sinkIndex)
  }
