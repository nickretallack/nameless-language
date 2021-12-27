@react.component
let make = (
  ~url: array<string>,
  ~autoSave: bool,
  ~execution: option<Execution.t>,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~emit,
) => {
  <nav className="left-right" id="nav">
    <div className="nav-buttons">
      <NavLink name="" text="Library" url />
      <NavLink name="+definition" text="+Definition" url />
      <NavLink name="html" text="HTML" url />
      <NavLink name="stack" text="Stack" url />
      {switch execution {
      | None => React.null
      | Some(execution) => <>
          <button onClick={_ => emit(AppAction.Step)}> {React.string("step")} </button>
          <button onClick={_ => emit(AppAction.Stop)}> {React.string("stop")} </button>
          {React.string(
            "Result: " ++ ValueDisplay.f(execution.result, execution, definitions, languageName),
          )}
        </>
      }}
    </div>
    <div> <AutoSaveCheckboxView autoSave={autoSave} emit /> </div>
  </nav>
}
