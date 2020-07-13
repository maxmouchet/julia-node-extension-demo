using PackageCompiler: bundle_artifacts, bundle_julia_libraries, create_pkg_context, create_sysimage
using Libdl

builddir = length(ARGS) > 0 ? ARGS[1] : "build"
project = joinpath(@__DIR__, "Clustering.jl")
sysimage_path = joinpath(builddir, "lib", "sysimage.$(Libdl.dlext)")

mkpath(builddir)

@info "Building Julia system image..." builddir=builddir project=project sysimage_path=sysimage_path

# From PackageCompiler.jl `create_app(...)`
ctx = create_pkg_context(project)
bundle_artifacts(ctx, builddir)
bundle_julia_libraries(builddir)

# Copy Project.toml (explain why)
cp(joinpath(project, "Project.toml"), joinpath(builddir, "lib", "Project.toml"))

create_sysimage(
    :Clustering,
    sysimage_path = sysimage_path,
    project = project,
    # Test only kmeans, others failing on Julia 1.5
    # TODO: Enable other tests
    precompile_execution_file = joinpath(project, "test", "kmeans.jl"),
    # precompile_execution_file = joinpath(project, "test", "runtests.jl"),
    # Optionally, to minimize the image size.
    # incremental = false,
    # filter_stdlibs = true,
    # Use `julia -C help` to list CPU targets.
    # cpu_target = skylake
)
