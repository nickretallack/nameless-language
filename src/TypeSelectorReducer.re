let f =
    (state: TypeSelectorState.t, action: TypeSelectorAction.t)
    : TypeSelectorState.t =>
  switch (action) {
  | Toggle => {...state, opened: !state.opened}
  | SelectCategory(category) => {...state, category}
  };
