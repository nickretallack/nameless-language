open Definition;

type createConnection = {
  source: connectionSide,
  sink: connectionSide,
};

type changeNibName = {
  nibID,
  isInput: bool,
  text: string,
};

type changeNibType = {
  nibID,
  isInput: bool,
  valueType,
};

type definitionAction =
  | CreateConnection(createConnection)
  | ChangeName(string)
  | ChangeDescription(string)
  | ChangeConstantValue(primitiveValue)
  | ChangeNibName(changeNibName)
  | ChangeNibType(changeNibType)
  | AddInput;

type definitionActionRecord = {
  definitionID,
  action: definitionAction,
};

type appAction =
  | DefinitionAction(definitionActionRecord)
  | ChangeRoute(ReasonReact.Router.url);
