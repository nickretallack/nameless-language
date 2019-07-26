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
      ~error: AppActions.appError,
      _children,
    ) => {
  ...component,
  render: _self => {
    let changeName = event => emit(ChangeName(getEventValue(event)));
    let uses =
      AffectedDefinitions.findDefinitionUses(definitionID, definitions);
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
             <option key=definitionID value=definitionID>
               {ReasonReact.string(getDisplayName(definition, "en"))}
             </option>,
           uses,
         )}
      </select>
      {switch (error) {
       | NoAppError => ReasonReact.null
       | NibIsConnected(definitions) =>
         <div>
           {ReasonReact.string("This nib is connected in: ")}
           <ul>
             {renderStringMap(
                ((definitionID, definition)) =>
                  <li>
                    <a key=definitionID href={"#" ++ definitionID}>
                      {ReasonReact.string(getDisplayName(definition, "en"))}
                    </a>
                  </li>,
                definitions,
              )}
           </ul>
         </div>
       | ConnectionCrossesScopeError =>
         <div>
           {ReasonReact.string(
              "When crossing scopes, you can only connect a source in a parent scope to a sink in a child scope.  You may have to remove some connections in order to change the scope of this node.",
            )}
         </div>
       }}
    </>;
  },
};
