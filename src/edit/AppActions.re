[%%debugger.chrome];
open Definition;

type appError =
  | NoAppError
  | NibIsConnected(definitions)
  | ConnectionCrossesScopeError;

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
  | ChangeNibOrdering(int)
  | RemoveNib;

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

type changeNodeScope = {
  nodeID,
  nodeScope,
};

type definitionAction =
  | CreateConnection(createConnection)
  | ChangeName(string)
  | ChangeDescription(string)
  | ChangeConstantValue(primitiveValue)
  | AddNode(addNode)
  | AddInput
  | AddOutput
  | NibAction(nibActionRecord)
  | RemoveConnection(connectionSide)
  | RemoveNodes(Belt.Set.String.t)
  | ChangeNodeScope(changeNodeScope)
  | Fork;

type definitionActionRecord = {
  definitionID,
  action: definitionAction,
};

type appAction =
  | DefinitionAction(definitionActionRecord)
  | CreateDefinition(definition)
  | ChangeRoute(ReasonReact.Router.url);
