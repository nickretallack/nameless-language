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

type nibAction =
  | ChangeNibName(string)
  | ChangeNibType(valueType)
  | ChangeNibOrdering(int);

type nibActionRecord = {
  nibID,
  isInput: bool,
  action: nibAction,
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
  | AddNode(addNode)
  | AddInput
  | AddOutput
  | NibAction(nibActionRecord);

type definitionActionRecord = {
  definitionID,
  action: definitionAction,
};

type appAction =
  | DefinitionAction(definitionActionRecord)
  | CreateDefinition(definition)
  | ChangeRoute(ReasonReact.Router.url);
