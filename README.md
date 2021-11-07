# FormalLanguagesTask5

#####Variant 6.

Given regexp alpha, letter x and natural number k. Answer if there exists a word in language L which contains exactly k lettes x.

#####Solution:

Dynamic stack while reading regexp. There two stacks: one for strings (just for understanding how algo works) and one for the number of occurrences of the required letter x in corresponding strings in first stack. Struct int_inf is a linear combination of some integers (for example, 5+6k+2l+13m+9n where k,l,m,n are positive integers and zero). This struct is necessary for accounting all the 'stars' in the regexp.
