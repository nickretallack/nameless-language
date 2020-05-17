[@react.component]
let make = (~autoSave: bool, ~emit) => {
  <>
    <label htmlFor="auto-save">
      <input
        id="auto-save"
        type_="checkbox"
        checked=autoSave
        onChange={_ => emit(AppAction.SetAutoSave(!autoSave))}
      />
      {ReasonReact.string("Auto Save")}
    </label>
    {ReasonReact.string(" ")}
    <button onClick={_ => emit(AppAction.ResetData)}>
      {ReasonReact.string("Reset Data")}
    </button>
  </>;
};
