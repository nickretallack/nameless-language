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

type addNode = {
  node,
  explicitConnectionSide,
  connectionNib,
};

type definitionAction =
  | CreateConnection(createConnection)
  | ChangeName(string)
  | ChangeDescription(string)
  | ChangeConstantValue(primitiveValue)
  | ChangeNibName(changeNibName)
  | ChangeNibType(changeNibType)
  | AddNode(addNode)
  | AddInput;

type definitionActionRecord = {
  definitionID,
  action: definitionAction,
};

type appAction =
  | DefinitionAction(definitionActionRecord)
  | CreateDefinition(definition)
  | ChangeRoute(ReasonReact.Router.url);
