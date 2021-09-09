type t =
  | PointerAction(PointerActionRecord.t)
  | SelectConnection(ConnectionSide.t)
  | SelectNode(SelectNode.t)
  | RemoveSelectedNodes
  | NewFunctionFromSelection
