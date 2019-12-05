type t = {
  scopes: Belt.Map.String.t(Scope.t),
  stack: list(Value.stackFrame),
  result: option(Value.t),
};
