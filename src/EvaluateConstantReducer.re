let f =
    (
      primitiveValue: PrimitiveValue.t,
      execution: Execution.t,
      frame: StackFrame.t,
      scope: Scope.t,
      source: ConnectionSide.t,
    )
    : Execution.t => {
  let value = Value.PrimitiveValue(primitiveValue);
  {
    ...execution,
    stack: [
      {...frame, action: Returning(value)},
      ...Belt.List.tailExn(execution.stack),
    ],
    scopes:
      Belt.Map.String.set(
        execution.scopes,
        frame.scopeID,
        {
          ...scope,
          sourceValues: Belt.Map.set(scope.sourceValues, source, value),
        },
      ),
  };
};
