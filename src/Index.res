// Entry point
switch ReactDOM.querySelector("#app") {
| Some(root) => ReactDOM.render(<AppView />, root)
| None => ()
}
