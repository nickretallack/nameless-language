[@react.component]
let make =
    (
      ~nibID: NibID.t,
      ~definitions: DefinitionMap.t,
      ~languageName: LanguageName.t,
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

  let changeType = (valueType: ValueType.t): unit => {
    Js.log2("CHANGE TYPE", valueType);
    emitNibAction(NibAction.ChangeNibType(valueType));
  };
  let changeOrdering = (index: int): unit =>
    emitNibAction(NibAction.ChangeNibOrdering(index));

  <tr>
    <td>
      <a onClick={_event => emitNibAction(NibAction.RemoveNib)}>
        {React.string("x")}
      </a>
    </td>
    <td>
      {index != 0
         ? <a onClick={_event => changeOrdering(index - 1)}>
             {React.string("^")}
           </a>
         : React.null}
    </td>
    <td>
      {index != count - 1
         ? <a onClick={_event => changeOrdering(index + 1)}>
             {React.string("v")}
           </a>
         : React.null}
    </td>
    <td>
      <input
        type_="text"
        className="name"
        value={TranslatableGetText.f(name, languageName)}
        onChange=changeName
      />
    </td>
    <td>
      <TypeSelectorView valueType definitions languageName changeType />
    </td>
  </tr>;
};
