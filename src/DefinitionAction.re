type t =
  | CreateDefinition(Definition.t)
  | CreateConnection(Connection.t)
  | ChangeName(string)
  | ChangeDescription(string)
  | ChangeConstantValue(PrimitiveValue.t)
  | AddNode(AddNode.t)
  | AddInput
  | AddOutput
  | NibAction(NibActionRecord.t)
  | RemoveConnection(ConnectionSide.t)
  | RemoveNodes(Belt.Set.String.t)
  | ChangeNodeScope(ChangeNodeScope.t)
  | Fork(DefinitionID.t)
  | EvaluateNib(ExplicitConnectionSide.t);
