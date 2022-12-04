function sum(a, b) begin
    return a + b
end

function main() begin
    var x, y, z

    x = 10
    y = 100
    z = sum(x, y)

    while (z > 0) begin
        println(z)
        z = z - 1
    end
end
