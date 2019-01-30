open Definition;
open Helpers;

let component = ReasonReact.statelessComponent("NibInterface");

let make =
    (
      ~nibID: nibID,
      ~definitions: definitions,
      ~isInput: bool,
      ~valueType: valueType,
      ~name: translatable,
      ~index: int,
      ~count: int,
      ~emit: AppActions.definitionAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let emitNibAction = action =>
      emit(AppActions.NibAction({nibID, isInput, action}));

    let changeName = event =>
      emitNibAction(AppActions.ChangeNibName(getEventValue(event)));

    let changeType = (valueType: valueType) =>
      emitNibAction(AppActions.ChangeNibType(valueType));

    let changeOrdering = (index: int) =>
      emitNibAction(AppActions.ChangeNibOrdering(index));

    <tr>
      <td>
        {index != 0 ?
           <a onClick={_event => changeOrdering(index - 1)}>
             {ReasonReact.string("^")}
           </a> :
           ReasonReact.null}
      </td>
      <td>
        {index != count - 1 ?
           <a onClick={_event => changeOrdering(index + 1)}>
             {ReasonReact.string("v")}
           </a> :
           ReasonReact.null}
      </td>
      <td>
        <input
          type_="text"
          className="name"
          value={getTranslated(name, "en")}
          onChange=changeName
        />
      </td>
      <td> <TypeSelector valueType definitions changeType /> </td>
    </tr>;
  },
};
