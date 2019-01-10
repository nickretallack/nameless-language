open Definition;

type createConnection = {
  source: connectionSide,
  sink: connectionSide,
};

type changeNibName = {
  nibID,
  text: string,
  isInput: bool,
};

type definitionAction =
  | CreateConnection(createConnection)
  | ChangeName(string)
  | ChangeDescription(string)
  | ChangeConstantValue(primitiveValue)
  | ChangeNibName(changeNibName)
  | AddInput;

type definitionActionRecord = {
  definitionID,
  action: definitionAction,
};

type appAction =
  | DefinitionAction(definitionActionRecord)
  | ChangeRoute(ReasonReact.Router.url);
