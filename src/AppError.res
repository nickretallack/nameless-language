type t =
  | NoAppError
  | NibIsConnected(DefinitionMap.t)
  | ConnectionCrossesScopeError
