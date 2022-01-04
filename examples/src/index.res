%raw(`require('./index.css')`)

@module("./serviceWorker")
external registerServiceWorker: unit => unit = "register"
@module("./serviceWorker")
external unregisterServiceWorker: unit => unit = "unregister"

switch ReactDOM.querySelector("#root") {
| Some(root) => ReactDOM.render(React.createElement(App.make, App.makeProps()), root)
| None => () // do nothing
}
