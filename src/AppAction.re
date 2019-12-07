type t =
  | DefinitionAction(DefinitionActionRecord.t)
  | CreateDefinition(Definition.t)
  | ChangeRoute(ReasonReact.Router.url)
  | Step;
