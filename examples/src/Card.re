[@bs.config {jsx: 3}];
[%bs.raw {|require('./Card.css')|}];
module Window = {
  type listenerId;

  [@bs.val] [@bs.scope "window"]
  external addWindowEventListener: (string, 'a => unit) => listenerId =
    "addEventListener";

  [@bs.val] [@bs.scope "window"]
  external removeWindowEventListener: (string, listenerId) => unit =
    "removeEventListener";

  [@bs.val] [@bs.scope "window"] external windowWidth: int = "innerWidth";
  [@bs.val] [@bs.scope "window"] external windowHeight: int = "innerHeight";
};
let useWindowSize = () => {
  open Window;
  let ((width, height), setSize) =
    ReactHooks.useState((windowWidth, windowHeight));
  ReactHooks.useEffect(
    () => {
      let handleResize = _ => setSize(. (windowWidth, windowHeight));
      let listener = addWindowEventListener("resize", handleResize);
      Some((.) => removeWindowEventListener("resize", listener));
    },
    [||],
  );
  (width, height);
};

module Hooks =
  Spring.MakeSpring({
    type t = (float, float, float);
    type interpolate = (float, float, float) => string;
  });

let trans = (x, y, s) => {j|perspective(600px) rotateX($(x)deg) rotateY($(y)deg) scale($s)|j};

[@react.component]
let make = () => {
  let (windowWidth, windowHeight) = useWindowSize();
  let calc = (x, y) => (
    -. (y->float_of_int -. windowHeight->float_of_int /. 2.) /. 20.,
    (x->float_of_int -. windowWidth->float_of_int /. 2.) /. 20.,
    1.05,
  );

  let (values, setValues) =
    Hooks.use(
      ~config=Spring.config(~mass=1., ~tension=80., ~friction=20.),
      (0., 0., 1.),
    );

  <Spring.Div
    className="card"
    onMouseMove={e => {
      let vals =
        calc(ReactEvent.Mouse.clientX(e), ReactEvent.Mouse.clientY(e));

      setValues(vals);
    }}
    onMouseLeave={() => setValues((0., 0., 1.))}
    style={ReactDOMRe.Style.make(
      ~transform=values->Hooks.interpolate(trans),
      (),
    )}
  />;
};