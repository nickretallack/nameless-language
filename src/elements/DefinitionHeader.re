[%%debugger.chrome];
open Definition;
open AppActions;
open Helpers;

let component = ReasonReact.statelessComponent("DefinitionHeader");

let make =
    (
      ~documentation: documentation,
      ~placeholder: string,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let changeName = event => emit(ChangeName(getEventValue(event)));

    <div>
      <input
        type_="text"
        className="name"
        placeholder
        value={getTranslated(documentation.name, "en")}
        onChange=changeName
      />
    </div>;
  },
};
