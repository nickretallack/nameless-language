open Definition;
open AppActions;
open Helpers;

let component = ReasonReact.statelessComponent("NibInterface");

let make =
    (
      ~nibID: nibID,
      ~isInput: bool,
      ~valueType: valueType,
      ~name: translatable,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let changeName = event =>
      emit(ChangeNibName({nibID, isInput, text: getEventValue(event)}));

    <div>
      <div> {ReasonReact.string("TODO: type")} </div>
      <input
        type_="text"
        className="name"
        value={getTranslated(name, "en")}
        onChange=changeName
      />
    </div>;
  },
};
