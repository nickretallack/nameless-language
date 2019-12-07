[@react.component]
let make =
    (
      ~definitionID: DefinitionID.t,
      ~definitions: DefinitionMap.t,
      ~documentation: Documentation.t,
      ~placeholder: string,
      ~emit: DefinitionAction.t => unit,
      ~error: AppError.t,
    ) => {
  let changeName = event => emit(ChangeName(EventGetValue.f(event)));
  let uses = DefinitionFindUses.f(definitionID, definitions);
  <>
    <div>
      <input
        type_="text"
        className="name"
        placeholder
        value={TranslatableGetText.f(documentation.name, "en")}
        onChange=changeName
      />
    </div>
    <button
      onClick={_event => {
        let newDefinitionID = RandomIDMake.f();
        emit(Fork(newDefinitionID));
        ReasonReactRouter.push("#" ++ newDefinitionID);
      }}>
      {ReasonReact.string("Fork")}
    </button>
    <select
      onChange={event =>
        ReasonReact.Router.push("#" ++ ReactEvent.Form.target(event)##value)
      }>
      <option> {ReasonReact.string("Uses...")} </option>
      {RenderStringMap.f(
         ((definitionID, definition)) =>
           <option key=definitionID value=definitionID>
             {ReasonReact.string(
                DefinitionGetDisplayName.f(definition, "en"),
              )}
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
           {RenderStringMap.f(
              ((definitionID, definition)) =>
                <li>
                  <a key=definitionID href={"#" ++ definitionID}>
                    {ReasonReact.string(
                       DefinitionGetDisplayName.f(definition, "en"),
                     )}
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
};
