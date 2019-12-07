[@react.component]
let make =
    (
      ~nibID: NibID.t,
      ~definitions: DefinitionMap.t,
      ~isInput: bool,
      ~valueType: ValueType.t,
      ~name: Translatable.t,
      ~index: int,
      ~count: int,
      ~emit: DefinitionAction.t => unit,
    ) => {
  let emitNibAction = action =>
    emit(DefinitionAction.NibAction({nibID, isInput, action}));

  let changeName = event =>
    emitNibAction(NibAction.ChangeNibName(EventGetValue.f(event)));

  let changeType = (valueType: ValueType.t) =>
    emitNibAction(NibAction.ChangeNibType(valueType));

  let changeOrdering = (index: int) =>
    emitNibAction(NibAction.ChangeNibOrdering(index));

  <tr>
    <td>
      <a onClick={_event => emitNibAction(NibAction.RemoveNib)}>
        {ReasonReact.string("x")}
      </a>
    </td>
    <td>
      {index != 0
         ? <a onClick={_event => changeOrdering(index - 1)}>
             {ReasonReact.string("^")}
           </a>
         : ReasonReact.null}
    </td>
    <td>
      {index != count - 1
         ? <a onClick={_event => changeOrdering(index + 1)}>
             {ReasonReact.string("v")}
           </a>
         : ReasonReact.null}
    </td>
    <td>
      <input
        type_="text"
        className="name"
        value={TranslatableGetText.f(name, "en")}
        onChange=changeName
      />
    </td>
    <td> <TypeSelectorView valueType definitions changeType /> </td>
  </tr>;
};
