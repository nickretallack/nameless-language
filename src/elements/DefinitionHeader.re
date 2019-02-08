[%%debugger.chrome];
open Definition;
open AppActions;
open Helpers;

let component = ReasonReact.statelessComponent("DefinitionHeader");

let make =
    (
      ~definitionID: definitionID,
      ~definitions: definitions,
      ~documentation: documentation,
      ~placeholder: string,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  render: _self => {
    let changeName = event => emit(ChangeName(getEventValue(event)));
    let uses = AffectedDefinitions.findUses(definitionID, definitions);
    <>
      <div>
        <input
          type_="text"
          className="name"
          placeholder
          value={getTranslated(documentation.name, "en")}
          onChange=changeName
        />
      </div>
      <button onClick={_event => emit(Fork)}>
        {ReasonReact.string("Fork")}
      </button>
      <select
        onChange={event =>
          ReasonReact.Router.push(
            "#" ++ ReactEvent.Form.target(event)##value,
          )
        }>
        <option> {ReasonReact.string("Uses...")} </option>
        {renderStringMap(
           ((definitionID, definition)) =>
             <option value=definitionID>
               {ReasonReact.string(getDisplayName(definition, "en"))}
             </option>,
           uses,
         )}
      </select>
    </>;
  },
};
