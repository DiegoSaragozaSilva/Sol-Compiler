function fibonacci(n) begin
    if (n == 1)
        return 1
    return n * fibonacci(n - 1)
end

function main() begin
    var n = 4
    println(fibonacci(n))
end
