// Entry point
switch ReactDOM.querySelector("#app") {
| Some(root) => ReactDOM.render(<React.StrictMode> <AppView /> </React.StrictMode>, root)
| None => ()
}
