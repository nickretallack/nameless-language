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
      <a href="#" className={url == [""] ? "active" : ""}> {React.string("Library")} </a>
      <a href="#+definition" className={url == ["+definition"] ? "active" : ""}>
        {React.string("+Definition")}
      </a>
      {switch execution {
      | None => React.null
      | Some(execution) => <>
          <button onClick={_ => emit(AppAction.Step)}> {React.string("step")} </button>
          {switch execution.result {
          | None => React.null
          | Some(value) =>
            React.string("Result: " ++ ValueDisplay.f(value, definitions, languageName))
          }}
        </>
      }}
    </div>
    <div> <AutoSaveCheckboxView autoSave={autoSave} emit /> </div>
  </nav>
}
