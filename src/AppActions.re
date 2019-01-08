open Definition;

type createConnection = {
  source: connectionSide,
  sink: connectionSide,
};

type changeInputName = {
  nibID,
  text: string,
};

type definitionAction =
  | CreateConnection(createConnection)
  | ChangeName(string)
  | ChangeDescription(string)
  | AddInput;

type definitionActionRecord = {
  definitionID,
  action: definitionAction,
};

type appAction =
  | DefinitionAction(definitionActionRecord)
  | ChangeRoute(ReasonReact.Router.url);
