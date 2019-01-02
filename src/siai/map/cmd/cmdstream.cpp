#include "map/cmd/cmdstream.hpp"
#include "map/cmd/cmdstreamimp.hpp"

CommandStream::CommandStream() = default;
CommandStream::~CommandStream() = default;

std::unique_ptr<CommandStream> CommandStream::create()
{
    return std::make_unique<CommandStreamImp>();
}
