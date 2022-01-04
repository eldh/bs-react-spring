[%bs.raw {|require('./index.css')|}];

[@bs.module "./serviceWorker"]
external registerServiceWorker: unit => unit = "register";
[@bs.module "./serviceWorker"]
external unregisterServiceWorker: unit => unit = "unregister";

switch (ReactDOM.querySelector("#root")) {
| Some(root) =>
  ReactDOM.render(React.createElement(App.make, App.makeProps()), root)
| None => () // do nothing
};
