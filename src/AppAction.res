type t =
  | CreateDefinition(Definition.t)
  | DefinitionAction(DefinitionActionRecord.t)
  | Step
  | Stop
  | SetAutoSave(bool)
  | ResetData
  | SetDefinitions(DefinitionMap.t)
