@react.component
let make = (~autoSave: bool, ~emit) => {
  <nav className="left-right" id="nav">
    <div id="nav-buttons">
      <a href="#"> {React.string("Library")} </a>
      <a href="#+definition"> {React.string("+Definition")} </a>
    </div>
    <div> <AutoSaveCheckboxView autoSave={autoSave} emit /> </div>
  </nav>
}
