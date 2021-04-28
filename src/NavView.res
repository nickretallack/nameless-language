@react.component
let make = (~url:array<string>, ~autoSave: bool, ~emit) => {
  <nav className="left-right" id="nav">
    <div className="nav-buttons">
      <a href="#" className={url==[""] ? "active" : ""}> {React.string("Library")} </a>
      <a href="#+definition" className={url==["+definition"] ? "active" : ""}> {React.string("+Definition")} </a>
    </div>
    <div> <AutoSaveCheckboxView autoSave={autoSave} emit /> </div>
  </nav>
}
