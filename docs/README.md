The syntax for identifiers is `[A-Za-z][A-Za-z0-9_]*`.

If an undefined symbol is encountered in a production, it is made into a new
nonterminal. This feature exists to allow circular dependencies. However,
misspelling a token name would result in the creation of a nonterminal. Prepend
the symbol identifier in a production with a `'` to force the program to treat
it as a token; an error will be signaled if the token does not exist.

## Special names:
 - `_end_` : The token signifying the end of the input
 - `_accept_` : The root nonterminal of the augmented grammar (the actual root)
 - `_root_` : The only production of the `_accept_` nonterminal
