let f = (
  changeType: ValueType.t => unit,
  state: TypeSelectorState.t,
  action: TypeSelectorAction.t,
): ReactUpdate.update<TypeSelectorAction.t, TypeSelectorState.t> =>
  switch action {
  | Toggle => ReactUpdate.Update({...state, opened: !state.opened})
  | SelectCategory(category) =>
    ReactUpdate.UpdateWithSideEffects(
      {...state, category: category},
      _ => {
        switch category {
        | AnyCategory => changeType(ValueType.AnyType)
        | TextCategory => changeType(ValueType.PrimitiveValueType(TextType))
        | NumberCategory => changeType(ValueType.PrimitiveValueType(NumberType))
        | _ => ()
        }
        None
      },
    )
  }
