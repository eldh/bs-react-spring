open Jest

test("addition", _ => {
  open Expect
  expect(3 + 4) |> toBe(7)
})
