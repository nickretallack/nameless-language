type t =
  | CreateDefinition(Definition.t)
  | DefinitionAction(DefinitionActionRecord.t)
  | Step
  | Stop
  | QueueEvaluation(QueueEvaluation.t)
  | SetAutoSave(bool)
  | ResetData
  | SetDefinitions(DefinitionMap.t)
