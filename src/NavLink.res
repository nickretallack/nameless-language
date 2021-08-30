@react.component
let make = (~url: array<string>, ~name: string, ~text: string) => {
  <a href={`#${name}`} className={url == [name] ? "active" : ""}> {React.string(text)} </a>
}
