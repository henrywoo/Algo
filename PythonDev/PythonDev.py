from collections import defaultdict
concat = ''.join

def lcs(xs, ys):
    '''Return a longest common subsequence of xs and ys.
    
    Example
    >>> lcs("HUMAN", "CHIMPANZEE")
    ['H', 'M', 'A', 'N']
    '''
    if xs and ys:
        *xb, xe = xs
        *yb, ye = ys
        if xe == ye:
            return lcs(xb, yb) + [xe]
        else:
            return max(lcs(xs, yb), lcs(xb, ys), key=len)
    else:
        return []


def count_lcs_calls(lcs):
    '''Return a pair (lcs, calls)
    
    Where:
    lcs - a wrapped version of lcs, which counts up calls
    calls - a dict mapping arg pairs to the number of times lcs
    has been called with these args.
    '''
    calls = defaultdict(int)
    def wrapped(xs, ys):
        calls[(concat(xs), concat(ys))] += 1
        return lcs(xs, ys)
    return wrapped, calls

lcs('MAN', 'PIG')
lcs, calls = count_lcs_calls(lcs)
print(*sorted((v, k) for k, v in calls.items()), sep='\n')

