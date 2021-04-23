@react.component
let make = (
  ~definitionID: DefinitionID.t,
  ~definitions: DefinitionMap.t,
  ~documentation: Documentation.t,
  ~languageName: LanguageName.t,
  ~placeholder: string,
  ~emit: DefinitionAction.t => unit,
  ~error: AppError.t,
) => {
  let changeName = event => emit(ChangeName(EventGetValue.f(event)))
  let uses = DefinitionFindUses.f(definitionID, definitions)
  <>
    <div>
      <input
        type_="text"
        className="name"
        placeholder
        value={TranslatableGetText.f(documentation.name, languageName)}
        onChange=changeName
      />
    </div>
    <button onClick={_event => emit(Fork)}> {React.string("Fork")} </button>
    <select
      onChange={event => RescriptReactRouter.push("#" ++ ReactEvent.Form.target(event)["value"])}>
      <option> {React.string("Uses...")} </option>
      {RenderStringMap.f(
        ((definitionID, definition)) =>
          <option key=definitionID value=definitionID>
            {React.string(DefinitionGetDisplayName.f(definition, languageName))}
          </option>,
        uses,
      )}
    </select>
    {switch error {
    | NoAppError => React.null
    | NibIsConnected(definitions) =>
      <div>
        {React.string("This nib is connected in: ")}
        <ul>
          {RenderStringMap.f(
            ((definitionID, definition)) =>
              <li>
                <a key=definitionID href={"#" ++ definitionID}>
                  {React.string(DefinitionGetDisplayName.f(definition, languageName))}
                </a>
              </li>,
            definitions,
          )}
        </ul>
      </div>
    | ConnectionCrossesScopeError =>
      <div>
        {React.string(
          "When crossing scopes, you can only connect a source in a parent scope to a sink in a child scope.  You may have to remove some connections in order to change the scope of this node.",
        )}
      </div>
    }}
  </>
}
