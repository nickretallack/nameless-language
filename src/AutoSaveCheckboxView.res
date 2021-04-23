@react.component
let make = (~autoSave: bool, ~emit) => <>
  <label htmlFor="auto-save">
    <input
      id="auto-save"
      type_="checkbox"
      checked=autoSave
      onChange={_ => emit(AppAction.SetAutoSave(!autoSave))}
    />
    {React.string("Auto Save")}
  </label>
  {React.string(" ")}
  <button onClick={_ => emit(AppAction.ResetData)}> {React.string("Reset Data")} </button>
</>
