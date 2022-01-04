[%bs.raw {|require('./Card.css')|}];

module SpringHook =
  Spring.MakeSpring({
    type t = (float, float, float);
    type interpolate = (float, float, float) => string;
  });

let trans = (x, y, s) => {j|perspective(600px) rotateX($(x)deg) rotateY($(y)deg) scale($s)|j};

[@react.component]
let make = () => {
  let (windowWidth, windowHeight) = Window.useWindowSize();
  let calc = (x, y) => (
    -. (y->float_of_int -. windowHeight->float_of_int /. 2.) /. 20.,
    (x->float_of_int -. windowWidth->float_of_int /. 2.) /. 20.,
    1.05,
  );

  let (values, setValues) =
    SpringHook.use(
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
    style={ReactDOM.Style.make(
      ~transform=values->SpringHook.interpolate(trans),
      (),
    )}
  />;
};