type t = {
  scopes: Belt.Map.String.t<Scope.t>,
  references: Belt.Map.String.t<Value.t>,
  stack: list<StackFrame.t>,
  result: Value.t,
  reactKey: string,
  debug: bool,
  scheduledEvents: ScheduledEventSet.t,
}
