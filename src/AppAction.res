type t =
  | CreateDefinition(Definition.t)
  | DefinitionAction(DefinitionActionRecord.t)
  | Step
  | SetAutoSave(bool)
  | ResetData
