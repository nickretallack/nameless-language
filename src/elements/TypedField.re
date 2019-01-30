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
      ~emit: AppActions.definitionAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let changeName = event =>
      emit(
        AppActions.NibAction({
          nibID,
          isInput,
          action: ChangeNibName(getEventValue(event)),
        }),
      );

    let changeType = (valueType: valueType) =>
      emit(
        AppActions.NibAction({
          nibID,
          isInput,
          action: AppActions.ChangeNibType(valueType),
        }),
      );

    <div>
      <TypeSelector valueType definitions changeType />
      <input
        type_="text"
        className="name"
        value={getTranslated(name, "en")}
        onChange=changeName
      />
    </div>;
  },
};
